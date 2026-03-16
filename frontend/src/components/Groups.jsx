import { useEffect, useState } from 'react'
import { Link } from 'react-router-dom'
import { useDispatch, useSelector } from 'react-redux'
import { fetchGroups } from '../store/groupSlice'
import CreateGroupModal from '../components/CreateGroupModal'

export default function Groups() {
    const dispatch = useDispatch()
    const { list: groups, loading } = useSelector(s => s.groups)
    const [showModal, setShowModal] = useState(false)

    useEffect(() => { dispatch(fetchGroups()) }, [dispatch])

    function handleGroupCreated() { setShowModal(false) }

    return (
        <div className="w-full px-8 lg:px-12 py-10">
            <div className="flex items-end justify-between gap-5 mb-8">
                <div>
                    <p className="text-xs font-medium text-white/25 uppercase tracking-widest mb-3">Your groups</p>
                    <h1 className="font-serif text-2xl sm:text-3xl text-white font-normal leading-snug">
                        Groups
                    </h1>
                    <p className="text-[13px] text-white/30 mt-1.5">
                        {groups.length > 0
                            ? `${groups.length} group${groups.length !== 1 ? 's' : ''}`
                            : 'No groups yet'}
                    </p>
                </div>
                <button
                    onClick={() => setShowModal(true)}
                    className="flex items-center gap-1.5 px-4 py-2.5 bg-emerald-500/15 hover:bg-emerald-500/25 border border-emerald-500/25 hover:border-emerald-500/40 text-emerald-400 text-sm font-medium rounded-xl transition-all duration-150 active:scale-[0.97]"
                >
                    <span className="text-base leading-none">+</span> New group
                </button>
            </div>

            <div className="border-t border-white/5 mb-8" />

            {loading ? (
                <div className="flex items-center justify-center py-24">
                    <div className="flex gap-1.5">
                        {[0,1,2].map(i => (
                            <div key={i} className="w-1.5 h-1.5 rounded-full bg-white/20 animate-pulse" style={{ animationDelay: `${i * 0.15}s` }} />
                        ))}
                    </div>
                </div>
            ) : groups.length === 0 ? (
                <div className="flex flex-col items-center gap-5 py-24 text-center">
                    <div className="w-14 h-14 rounded-2xl bg-white/4 border border-white/7 flex items-center justify-center">
                        <span className="font-serif text-3xl text-white/20">÷</span>
                    </div>
                    <div>
                        <p className="text-white/50 text-sm font-medium mb-1">No groups yet</p>
                        <p className="text-white/25 text-xs">Create one to start splitting expenses</p>
                    </div>
                    <button onClick={() => setShowModal(true)}
                        className="px-5 py-2.5 bg-emerald-500/15 hover:bg-emerald-500/25 border border-emerald-500/25 text-emerald-400 font-medium rounded-xl text-sm transition-all">
                        Create your first group
                    </button>
                </div>
            ) : (
                <div className="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3 2xl:grid-cols-4 gap-4">
                    {groups.map((group, i) => (
                        <Link key={group.id} to={`/groups/${group.id}`}
                            className="group flex items-center gap-3.5 p-4 bg-white/3 hover:bg-white/5 border border-white/6 hover:border-white/12 rounded-2xl transition-all duration-150"
                            style={{ animationDelay: `${i * 0.05}s` }}
                        >
                            <div className="w-10 h-10 rounded-xl bg-emerald-500/10 border border-emerald-500/20 flex items-center justify-center font-serif text-lg text-emerald-400 shrink-0">
                                {group.name[0].toUpperCase()}
                            </div>
                            <div className="flex-1 min-w-0">
                                <h3 className="text-[13.5px] font-medium text-white truncate">{group.name}</h3>
                                <p className="text-xs text-white/30 mt-0.5">
                                    {group.members?.length ?? 0} members
                                    {group.createdBy && ` · ${group.createdBy.name}`}
                                </p>
                            </div>
                            <span className="text-white/20 group-hover:text-white/50 text-sm transition-colors">→</span>
                        </Link>
                    ))}
                </div>
            )}

            {showModal && (
                <CreateGroupModal onClose={() => setShowModal(false)} onCreated={handleGroupCreated} />
            )}
        </div>
    )
}