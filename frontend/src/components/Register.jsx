/* eslint-disable react-hooks/exhaustive-deps */
import { useState, useEffect } from 'react'
import { useNavigate, Link } from 'react-router-dom'
import { useAuth } from '../context/AuthContext'

export default function Register() {
    const [form, setForm]= useState({ name: '', email: '', password: '' })
    const [localErr, setLocalErr] = useState('')
    const { register, loading, error, token } = useAuth()
    const navigate = useNavigate()

    useEffect(() => { 
        if (token) {
            navigate('/dashboard')
        } 
    }, [token])

    const handleChange = e => setForm(f => ({ ...f, [e.target.name]: e.target.value }))

    async function handleSubmit(e) {
        e.preventDefault()
        setLocalErr('')
        if (form.password.length < 6){ 
            setLocalErr('Password must be at least 6 characters'); 
            return 
        }
        const result = await register(form)
        if (!result.error) {
            navigate('/dashboard')
        }
    }

    const inputClass = "bg-[#1e2026] border border-[#2a2d35] text-white rounded-lg px-3.5 py-2.5 text-sm focus:outline-none focus:border-emerald-400 transition-colors placeholder:text-[#5c6070]"

    return (
        <div className="min-h-screen bg-[#0e0f11] flex items-center justify-center p-6">
        <div className="w-full max-w-sm fade-up">
            <div className="flex items-center gap-2.5 mb-10">
            <span className="font-serif text-3xl text-emerald-400">÷</span>
            <span className="font-serif text-xl text-white">splitwise</span>
            </div>

            <h1 className="font-serif text-3xl text-white font-normal mb-1.5">Create account</h1>
            <p className="text-sm text-[#9a9da8] mb-8">Start splitting expenses with friends</p>

            <form onSubmit={handleSubmit} className="flex flex-col gap-4">
            <div className="flex flex-col gap-1.5">
                <label className="text-xs font-medium text-[#9a9da8] uppercase tracking-wider">Name</label>
                <input name="name" placeholder="Alice" value={form.name} onChange={handleChange} className={inputClass} />
            </div>
            <div className="flex flex-col gap-1.5">
                <label className="text-xs font-medium text-[#9a9da8] uppercase tracking-wider">Email</label>
                <input name="email" type="email" placeholder="you@example.com" value={form.email} onChange={handleChange} className={inputClass} />
            </div>
            <div className="flex flex-col gap-1.5">
                <label className="text-xs font-medium text-[#9a9da8] uppercase tracking-wider">Password</label>
                <input name="password" type="password" placeholder="Min 6 characters" value={form.password} onChange={handleChange} className={inputClass} />
            </div>

            {(localErr || error) && (
                <div className="text-sm text-red-400 bg-red-400/10 border border-red-400/20 rounded-lg px-3.5 py-2.5">
                {localErr || error}
                </div>
            )}

            <button
                type="submit" disabled={loading}
                className="w-full h-11 mt-1 bg-emerald-400 hover:bg-emerald-300 text-[#0a1a13] font-semibold rounded-lg text-sm transition-all disabled:opacity-40 disabled:cursor-not-allowed"
            >
                {loading ? 'Creating account...' : 'Create account'}
            </button>
            </form>

            <p className="mt-7 text-sm text-[#9a9da8] text-center">
            Already have an account?{' '}
            <Link to="/login" className="text-emerald-400 font-medium hover:underline">Sign in</Link>
            </p>
        </div>
        </div>
    )
}